import { useEffect, useState } from "react";
import "../App.css";
import {
  ActionIcon,
  Button,
  Checkbox,
  Divider,
  Flex,
  Input,
  Modal,
  Select,
  Table,
} from "@mantine/core";
import useStore, { Params } from "../store";
import IconAdd from "./IconAdd";

interface Props {
  open: boolean;
  toggleOpen: () => void;
  uuid?: string;
  isFunction: boolean;
}

export function useFormOfMethods() {
  const [open, setOpen] = useState<boolean>(false);
  const [uuid, setUUID] = useState<string>("");

  function toggleOpen() {
    setOpen(!open);
  }

  function cb() {
    toggleOpen();
  }

  return { open, toggleOpen, cb, setUUID, uuid };
}

export default function FormOfMethods({
  open,
  toggleOpen,
  uuid,
  isFunction,
}: Props) {
  const [method, setMethod] = useState<string>("");
  const [methodEng, setMethodEng] = useState<string>("");
  const [params, setParams] = useState<Params[]>([]);
  const addMethod = useStore((state) => state.addMethod);
  const saveMethod = useStore((state) => state.saveMethod);
  const methods = useStore((state) => state.methods);
  const [returnType, setReturnType] = useState<string>("");
  const [hasReturn, setHasReturn] = useState<boolean>(false);
  const [callRustMethod, setCallRustMethod] = useState<boolean>(false);

  function onCancel() {
    toggleOpen();
  }

  function onSave() {
    if (uuid) {
      saveMethod({
        uuid: uuid,
        name: method,
        nameEng: methodEng,
        params: [...params],
        ReturnType: returnType,
        hasReturn: hasReturn,
        callRustMethod: callRustMethod,
      });

      toggleOpen();

      return;
    }

    addMethod({
      uuid: crypto.randomUUID(),
      name: method,
      nameEng: methodEng,
      params: [...params],
      ReturnType: returnType,
      hasReturn: hasReturn,
      callRustMethod: callRustMethod,
    });

    toggleOpen();
  }

  useEffect(() => {
    setMethod(
      methods.find((method) => method.uuid === uuid)?.name ||
        `method${methods.length + 1}`
    );

    setMethodEng(
      methods.find((method) => method.uuid === uuid)?.nameEng ||
        `methodEng${methods.length + 1}`
    );

    setReturnType(
      methods.find((method) => method.uuid === uuid)?.ReturnType || "string"
    );

    setParams(methods.find((method) => method.uuid === uuid)?.params || []);

    setHasReturn(
      methods.find((method) => method.uuid === uuid)?.hasReturn || isFunction
    );

    if (uuid) {
      setCallRustMethod(
        methods.find((method) => method.uuid === uuid)?.callRustMethod || false
      );
    } else {
      setCallRustMethod(true);
    }
  }, []);

  function onChangeType(selectedValue: string | null, uuid: string) {
    if (!selectedValue) return;
    setParams(
      params.map((item) =>
        item.uuid === uuid ? { ...item, type: selectedValue } : item
      )
    );
  }

  function onChangeReturnType(selectedValue: string | null) {
    if (!selectedValue) return;
    setReturnType(selectedValue);
  }

  function addParam(): void {
    setParams([
      ...params,
      {
        uuid: crypto.randomUUID(),
        name: `param${params.length + 1}`,
        type: "string",
      },
    ]);
  }

  function setParamName(value: string, uuid: string) {
    setParams(
      params.map((item) =>
        item.uuid === uuid ? { ...item, name: value } : item
      )
    );
  }

  return (
    <Modal
      opened={open}
      onClose={onCancel}
      title={uuid ? "Редактировать метод" : "Новый метод"}
      transitionProps={{ duration: 0 }}
      overlayProps={{ blur: 0.25 }}
    >
      <Divider label="Имя метода" />

      <Input.Wrapper label="Имя метода на русском">
        <Input
          value={method}
          onChange={(e) => setMethod(e.target.value)}
          variant="filled"
        />
      </Input.Wrapper>

      <Input.Wrapper label="Имя метода на английском">
        <Input
          value={methodEng}
          onChange={(e) => setMethodEng(e.target.value)}
          variant="filled"
        />
      </Input.Wrapper>

      <Checkbox
        checked={callRustMethod}
        onChange={(e) => setCallRustMethod(e.target.checked)}
        label="Вызов Rust метода (FFI)"
        mt={10}
      />

      <Divider label="Параметры метода" mt={5} />

      <Flex ml={5} mb={-10}>
        <ActionIcon onClick={addParam} variant="subtle">
          <IconAdd />
        </ActionIcon>
      </Flex>

      <Table withColumnBorders mt={10}>
        <Table.Thead>
          <Table.Tr>
            <Table.Th>Имя</Table.Th>
            <Table.Th>Тип</Table.Th>
          </Table.Tr>
        </Table.Thead>
        <Table.Tbody>
          {params.map((i) => (
            <Table.Tr key={i.uuid}>
              <Table.Td>
                <Input
                  variant="filled"
                  value={i.name}
                  onChange={(e) => setParamName(e.target.value, i.uuid)}
                />
              </Table.Td>
              <Table.Td>
                <Select
                  value={i.type}
                  onChange={(e) => {
                    onChangeType(e, i.uuid);
                  }}
                  data={["string", "number", "bool"]}
                ></Select>
              </Table.Td>
            </Table.Tr>
          ))}
        </Table.Tbody>
      </Table>
      <Divider label="Возврат (для функции)" mt={5} />
      <Checkbox
        checked={hasReturn}
        onChange={(e) => setHasReturn(e.target.checked)}
        label="Возвращает значение"
        mt={10}
      />

      {hasReturn && (
        <Input.Wrapper label="Тип возвращаемого значения">
          <Select
            value={returnType}
            onChange={(e) => {
              onChangeReturnType(e);
            }}
            data={["string"]}
          ></Select>
        </Input.Wrapper>
      )}

      <Flex mt={10} justify={"end"}>
        <Button onClick={onSave}>Save</Button>
      </Flex>

      <div></div>
    </Modal>
  );
}
